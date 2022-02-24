Στην εργασια αυτη συμμετειχαν οι:
Νικολαος Μιχαλουτσος (Αριθμος Μητρωου: 1115202000133)
Ανδρεας Ταξιαρχης Κοκκορας (Αριθμος Μητρωου: 1115202000085)

Compile with: g++ <program_name> OOP-Project.cpp roads.cpp roads.h
Execute with: ./<program_name> <arg1> <arg2> <arg3> <arg4>

Executed with arguments such as: 
    8 3 15 100 (100 for capacity in each section)
    8 3 25 100 (100 for capacity in each section)
    8 3 250 100 (100 for capacity in each section)
    8 3 3 100 (100 for capacity in each section)
    15 10 50 50 (cap1 = 100, cap2 = 200, cap3 = 400, cap4 = 800, cap5 = 1600, cap6 = 1000, cap7 = 500, cap8 = 200, cap9 = 200, cap10 = 100)
    8 3 25 50 (cap1 = 1000, cap2 = 200, cap3 = 400)
    11 11 11 11 (cap1 = 130, cap2 = 130, cap3 = 300, cap4 = 300, cap5 = 1000, cap6 = 1000, cap7 = 300, cap8 = 300, cap9 = 200, cap10 = 1000, cap11 = 600)

! Ισως χρειαστει να σχολιαστει το μηνυμα του constructor και του destructor των vehicles για να φανουν ολα τα μηνυματα.

Κλασεις:

• Highway: Κρατα τον αριθμο των οχηματων, τον αριθμο τμηματων και τον πινακα με τα τμηματα. Η operate καλει ουσιαστικα τις operate του καθε τμηματος απ το τελος
    προς την αρχη για να γινει operate ολη η Αττικη οδος.

• Section: Η μεταβλητη delays χρησιμοποιειται για την εκτυπωση μηνυματων τυχον καθυστερησης της μεταβασης οχηματων απο το προηγουμενο σε εκεινο που βρισκομαστε.
    Επισης κραταει το index του, αριθμο οχηματων του τμηματος, ποσοστο οχηματων που θα γινονται ετοιμα, χωρητικοτητα τμηματος, αριθμο τμηματων, τον κομβο απ τον οποιο μπαινουν τα οχηματα στο τμημα, προηγουμενο κι επομενο τμημα και τον πινακα με τα οχηματα. Ο τελευταιος κομβος (μετα το τελευταιο τμημα δηλαδη) ειναι υποθετικος, για αυτο οι πραγματικοι κομβοι ειναι οσοι και τα τμηματα. Η shift() θα εξηγηθει αργοτερα.

• Node: Παρομοια με τη Section η μεταβλητη delays χρησιμοποιειται για την εκτυπωση μηνυματων τυχον καθυστερησης της μεταβασης οχηματων απο τον κομβο του τμηματος σε αυτο.
    Η μεταβλητη current_toll_with_priority χρησιμοποιειται στην give_vehs_to_the_section() και εξηγειται αργοτερα. Η max_tolls_category οριζεται για τον μεγιστο αριθμο διοδιων σε καθε κατηγορια. Η operate ουσιαστικα δεν πειραζει τιποτα στον κομβο παρα μονο κανει operate τα διοδια του.

• Toll: Η MAX_VEHS_INSERT_IN_TOLL αναπαριστα τον αριθμο οχηματων που μπορει να δεχθει το καθε διοδιο. Η fix_matrix_reduce() και η my_realloc() θα εξηγηθει αργοτερα.

• Vehicle: Τα section_index και node_index δεν χρησιμοποιουνται πουθενα. Τα εχουμε λογω εκφωνησης. Το destination αναπαριστα το τμημα που θελει να φυγει απο κομβο το οχημα.

Λειτουργια:

• Section::exit(): Την εξοδο των οχηματων απο την Αττικη Οδο απο κομβο τη θεωρουμε υποθετικη, απλα τα κανουμε delete.

• Section::enter(): Προτεραιοτητα εχουν τα οχηματα απο το προηγουμενο τμημα και μετα αυτα απ τους κομβους.

• Section::shift(): Γεμιζει ολες τις κενες θεσεις (αν υπαρχουν) αντικαθιστωντας τες με οχηματα απο τις επομενες θεσεις. Ετσι ουσιαστικα, ολα τα οχηματα "προχωρουν" το ενα πισω
    απο το αλλο.

• Στην section::operate() επιλεγονται τα percent% των οχηματων που τυχαινει να ειναι πρωτα στον πινακα να γινουν "ετοιμα".

• Node::give_vehs_to_the_section(): Θεωρουμε οτι η συναρτηση ισως μπερδευει οποτε θα εξηγηθει εδω. Αρχικα η συναρτηση επιστρεφει NULL οταν δεν θα μεταβιβασει οχημα στο
    τμημα. Αυτο γινεται ειτε οταν δεν χωραει κανενα οχημα στο τμημα ειτε οταν δεν εχει οχημα να δωσει κανενα διοδιο. Διαφορετικα επιστρεφει εναν πινακα με οχηματα που θα περασουν στο τμημα. Ο τροπος που τα διοδια δινουν οχηματα ειναι ο εξης: καθε διοδιο δινει απο ενα οχημα τη φορα και προχωραμε στο επομενο (κυκλικα εννοειται) μεχρι να μη χωραει αλλα οχηματα το τμημα ή να τελειωσουν τα οχηματα σε ολα τα διοδια. Καθε φορα που θα καλουμε τη συναρτηση αυτη θα ξεκιναμε απο το διοδιο που ειχαμε μεινει την προηγουμενη φορα, οχι απο το πρωτο παλι. Αυτο ελεγχεται και πραγματοποιειται με την μεταβλητη current_toll_with_priority. Το αν ολα τα διοδια ξεμειναν απο οχηματα το ελεγχουμε με την μεταβλητη tolls_not_giving_car η οποια αυξανεται καθε φορα που ενα διοδιο ΔΕΝ δινει οχημα και μηδενιζεται καθε φορα που δινει. Αν αυτη η μεταβλητη γινει ιση με τον αριθμο των διοδιων τοτε ξεμειναν ολα. Αφου τελειωσουμε με τη συλλογη των οχηματων που θα μπουν στο τμημα, ενημερωνουμε το current_toll_with_priority για την επομενη φορα, ενημερωνουμε το delays για την εκτυπωση μηνυματων, και το k.
    Επισης, στον constructor του node κανουμε τα πρωτα num_of_electronics ηλεκτρονικα διοδια. Για αυτο και οταν το διοδιο που βρισκομαστε ειναι < num_of_electronics τοτε καλουμε την toll::give_a_veh() με παραμετρο 2*k.
    
• Toll::fix_matrix_reduce(): Φτιαχνει τον πινακα με τα οχηματα του διοδιου. Οταν δινει οχημα το διοδιο (give_a_veh()) δινει το πρωτο, αρα το αντικαθιστουμε με το τελευταιο του
    πινακα.

• Toll::my_realloc(): Ο πινακας με τα οχηματα των διοδιων ειναι μεταβλητου μεγεθους που προσαρμοζεται απο την my_realloc(). Οταν ο πινακας προβλεπεται να υπερχειλισει η
    χωρητικοτητα του διπλασιαζεται (κι αν παλι δεν αρκει ξαναδιπλασιαζεται κοκ). Οταν ο πινακας εχει οχηματα οσο και το 1/4 της χωρητικοτητας του τοτε αυτη υποδιπλασιαζεται για εξοικονομηση χωρου.

• Καθε φορα που δημιουργουμε ενα νεο οχημα του βαζουμε destination την τιμη: index + rand() % (nsegs - index)). Αυτη η τιμη αντιστοιχει σε μια τυχαια τιμη αναμεσα στο index
    του τμηματος (ή το index του κομβου ενος τμηματος) στο οποιο δημιουργειται, και στο nsegs - 1. Π.χ. Αν δημιουργηθει οχημα στο τμημα με index 3 κι υπαρχουν 9 τμηματα, τοτε θα παρει destination απο 4 εως 8.

